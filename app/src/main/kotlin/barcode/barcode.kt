// Created by Loris Witschard on 4/21/2018.
package barcode

object Barcode
{
    private val packets = mutableListOf<Packet>()
    
    fun addPacket(packet: Packet)
    {
        packets.add(packet)
    }
    
    fun clear()
    {
        packets.clear()
    }
    
    fun serialize(): Array<Boolean>
    {
        val bits = mutableListOf<Boolean>()
        
        bits.addAll(Packet(Action.SYNC).serialize())
        packets.forEach { packet -> bits.addAll(packet.serialize()) }
        bits.addAll(Packet(Action.END).serialize())
        
        return bits.toTypedArray()
    }
    
    override fun toString(): String
    {
        return packets.joinToString()
    }
}
